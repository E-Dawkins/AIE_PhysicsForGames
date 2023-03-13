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
		if(Input.GetMouseButton(0) && CanShoot())
		{
			if(shootAnimation.isPlaying)
				return;
			
			// Spawn muzzle flash effect
			m_muzzleFlashCR ??= StartCoroutine(MuzzleFlash().GetEnumerator());

			// Play shot animation
			shootAnimation.Play();

			Vector3 shotOffset = new Vector3(1, 1, 0) * Random.Range(-gunType.bulletSpread, gunType.bulletSpread);
            
			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
			ray.direction = Quaternion.Euler(shotOffset) * ray.direction;

			// Checks if we are aiming at an object, using the random degree offset
			if(Physics.Raycast(ray, out RaycastHit hit, Mathf.Infinity, shotLayerMask.value, QueryTriggerInteraction.Ignore))
			{
				// Bullet hit an enemy
				Zombie zombie = hit.collider.GetComponentInParent<Zombie>();

				if(zombie != null)
				{
					float damage = hit.collider == zombie.HeadCollider ? 
						               gunType.headDamage : gunType.bodyDamage;

					zombie.health -= damage;

					player.score += zombie.health > 0 ? 10 : 100;
				}
			}
		}
	}

	private IEnumerable MuzzleFlash()
	{
		ParticleSystem effect = Instantiate(muzzleFlash, muzzleFlashTransform);
		effect.Play();

		yield return new WaitForSeconds(effect.main.duration);
		
		Destroy(effect.gameObject);

		m_muzzleFlashCR = null;
	}
}