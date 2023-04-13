using System.Collections;
using UnityEngine;
using Random = UnityEngine.Random;

public class Gun : MonoBehaviour
{
	public GunType gunType;
	[SerializeField] private FPSController player;

	[SerializeField] private ParticleSystem muzzleFlash;
	[SerializeField] private Transform muzzleFlashTransform;
	
	[SerializeField] private Animation shootAnimation;
	[SerializeField] private LayerMask shotLayerMask;

	private float m_maxDelay;
	private float m_currentDelay;

	private Coroutine m_muzzleFlashCR;

	private void Awake() => m_maxDelay = 1f / gunType.fireRate;

	private void Update() => m_currentDelay += Time.deltaTime;

	private bool CanShoot()
	{
		bool canShoot = false;

		if(m_currentDelay >= m_maxDelay)
		{
			// Gun is automatic, so can shoot
			if (gunType.automatic)
				canShoot = true;

			// Gun is not automatic, player has to re-click to shoot
			if (Input.GetMouseButtonDown(0))
				canShoot = true;

			// Only reset the delay if the player is about to shoot
			if(canShoot) m_currentDelay = 0;
		}

		return canShoot;
	}
	
	public void GunLogic()
	{
		// Only shoot if fire button is pressed and can shoot
		if(Input.GetMouseButton(0) && CanShoot())
		{
			// Don't shoot if recoil animation is playing
			if(shootAnimation.isPlaying)
				return;
			
			// Spawn muzzle flash effect
			m_muzzleFlashCR ??= StartCoroutine(MuzzleFlash().GetEnumerator());

			// Play shot animation
			shootAnimation.Play();

			// Shot offset
			Vector3 shotOffset = new Vector3(1, 1, 0) * Random.Range(-gunType.bulletSpread, gunType.bulletSpread);
            
			// Ray from screen center is offset by a few degrees
			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
			ray.direction = Quaternion.Euler(shotOffset) * ray.direction;

			// Checks if we are aiming at an object, using the random degree offset
			if(Physics.Raycast(ray, out RaycastHit hit, Mathf.Infinity, shotLayerMask.value, QueryTriggerInteraction.Ignore))
			{
				// Bullet hit an enemy
				Zombie zombie = hit.collider.GetComponentInParent<Zombie>();

				if(zombie != null)
				{
					// Damage multiplier for body / head shot
					float damage = hit.collider == zombie.HeadCollider ? 
						               gunType.headDamage : gunType.bodyDamage;

					zombie.health -= damage;

					// Player score for damaging the enemy or killing the enemy
					player.score += zombie.health > 0 ? 10 : 100;
				}
			}
		}
	}

	private IEnumerable MuzzleFlash()
	{
		// Spawn muzzle flash effect
		ParticleSystem effect = Instantiate(muzzleFlash, muzzleFlashTransform);
		effect.Play();

		// Wait until the effect finishes
		yield return new WaitForSeconds(effect.main.duration);
		
		// Then destroy the effect gameobject
		Destroy(effect.gameObject);

		// And set the coroutine to null
		m_muzzleFlashCR = null;
	}
}