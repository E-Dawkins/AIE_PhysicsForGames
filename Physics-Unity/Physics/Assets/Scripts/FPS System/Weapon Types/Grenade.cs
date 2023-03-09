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
		m_rb = GetComponent<Rigidbody>();
		m_rb.isKinematic = true;
	}

	private void Update()
	{
		if (m_shouldCook)
			m_curCookTime += Time.deltaTime;

		// Explosion
		if(m_curCookTime >= cookTime)
		{
			m_explosionCR ??= StartCoroutine(Explosion().GetEnumerator());
		}
	}

	public void ThrowGrenade()
	{
		m_rb.isKinematic = false;
		GetComponent<Collider>().enabled = true;
		
		Vector3 force = Camera.main.transform.forward + Camera.main.transform.up * 0.25f;
		m_rb.AddForceAtPosition(force * throwForce, transform.position + transform.up * 0.2f);
		
		transform.SetParent(null);
	}

	private void CheckExplosionRadius()
	{
		List<Zombie> damagedEnemies = new List<Zombie>();

		foreach(Collider coll in Physics.OverlapSphere(transform.position, explosionRadius, grenadeLayerMask, QueryTriggerInteraction.Ignore))
		{
			float percentFromCenter = Vector3.Distance(coll.transform.position, transform.position) / explosionRadius;
			float damage = Mathf.Clamp01(1 - percentFromCenter) * maxDamage;

			// If collider is an enemy and they have not already been damaged
			Zombie zombie = coll.GetComponentInParent<Zombie>();
			
			if(zombie != null && !damagedEnemies.Contains(zombie))
			{
				zombie.health -= damage;

				player.score += zombie.health > 0 ? 50 : 200;

				// Set enemy to ragdoll
				Ragdoll enemyRd = zombie.GetComponent<Ragdoll>();
				enemyRd.RagdollOn = true;
				
				// Make them fly
				enemyRd.AddExplosionForce(explosionForce, transform.position, explosionRadius);

				damagedEnemies.Add(zombie);
			}

			else if(coll.TryGetComponent(out FPSController fpsController))
				fpsController.health -= damage;
		}
		
		damagedEnemies.Clear();
	}

	private IEnumerable Explosion()
	{
		GetComponent<MeshRenderer>().enabled = false;
		
		// Effect
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