using System.Collections;
using System.Collections.Generic;

using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class Grenade : MonoBehaviour
{
	[SerializeField] private float cookTime = 3;
	[SerializeField] private float throwForce = 500;
	[SerializeField] private float maxDamage = 150;
	[SerializeField] private float explosionRadius = 5;
	[SerializeField] private float explosionForce = 100;
	[SerializeField] private LayerMask grenadeLayerMask;
	[SerializeField] private ParticleSystem explosionEffect;

	[SerializeField] private FPSController player;

	public bool ShouldCook
	{
		set => m_shouldCook = value;
	}
	
	private bool m_shouldCook;

	private float m_curCookTime;
	private Rigidbody m_rb;
	
	private Coroutine m_explosionCR;

	private void Awake()
	{
		// Initialise rigidbody variable and set it to be non-kinematic
		m_rb = GetComponent<Rigidbody>();
		m_rb.isKinematic = true;
	}

	private void Update()
	{
		// Increment cook timer
		if (m_shouldCook)
			m_curCookTime += Time.deltaTime;

		// If cook timer is greater than the max cook time, run the explosion coroutine
		if(m_curCookTime >= cookTime)
		{
			m_explosionCR ??= StartCoroutine(Explosion().GetEnumerator());
		}
	}

	public void ThrowGrenade()
	{
		// Set rigidbody to non-kinematic and enable the grenades collider
		m_rb.isKinematic = false;
		GetComponent<Collider>().enabled = true;
		
		// The force that is used to throw the grenade
		Vector3 force = Camera.main.transform.forward + Camera.main.transform.up * 0.25f;
		m_rb.AddForceAtPosition(force * throwForce, transform.position + transform.up * 0.2f);
		
		// Sever child-parent relationship
		transform.SetParent(null);
	}

	private void CheckExplosionRadius()
	{
		// A list of damaged zombies that are within the explosion radius
		List<Zombie> damagedEnemies = new List<Zombie>();

		// Check each collider within the explosion radius
		foreach(Collider coll in Physics.OverlapSphere(transform.position, explosionRadius, grenadeLayerMask, QueryTriggerInteraction.Ignore))
		{
			// Damage calculated from the percent the object is from the explosion center
			float percentFromCenter = Vector3.Distance(coll.transform.position, transform.position) / explosionRadius;
			float damage = Mathf.Clamp01(1 - percentFromCenter) * maxDamage;

			// If collider is an enemy and they have not already been damaged...
			Zombie zombie = coll.GetComponentInParent<Zombie>();
			
			if(zombie != null && !damagedEnemies.Contains(zombie))
			{
				// ...then damage the zombie, and increment the players score
				zombie.health -= damage;
				player.score += zombie.health > 0 ? 50 : 200;

				// Set enemy to ragdoll
				Ragdoll enemyRd = zombie.GetComponent<Ragdoll>();
				enemyRd.RagdollOn = true;
				
				// Make them fly
				enemyRd.AddExplosionForce(explosionForce, transform.position, explosionRadius);

				// Add the damaged zombie to the list of damaged zombies
				damagedEnemies.Add(zombie);
			}

			// If the player is within the explosion radius, damage them
			else if(coll.TryGetComponent(out FPSController fpsController))
				fpsController.health -= damage;
		}
		
		// Clear the list, frees memory?
		damagedEnemies.Clear();
	}

	// Explosion coroutine
	private IEnumerable Explosion()
	{
		// Set the grenade mesh to not be rendered
		GetComponent<MeshRenderer>().enabled = false;
		
		// Spawn and play particle effect
		ParticleSystem effect = Instantiate(explosionEffect, transform.position, Quaternion.identity);
		effect.Play();

		// Check each collider in explosion radius to see if they are enemy or player
		CheckExplosionRadius();

		// Wait until effect has finished
		yield return new WaitForSeconds(explosionEffect.main.duration);
		
		// Destroy the effect and grenade
		Destroy(effect.gameObject);
		Destroy(gameObject);
	}
}