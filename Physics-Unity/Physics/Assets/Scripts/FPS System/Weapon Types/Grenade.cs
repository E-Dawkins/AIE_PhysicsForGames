using System;
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

	public bool ShouldCook
	{
		set => m_shouldCook = value;
	}
	private bool m_shouldCook;

	private float m_curCookTime;
	private Rigidbody m_rb;

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
			// Effect
			
			
			// Sound
			

			// Check each collider in explosion radius to see if they are enemy or player
			CheckExplosionRadius();

			// Destroy the grenade
			Destroy(gameObject);
		}
	}

	public void ThrowGrenade()
	{
		m_rb.isKinematic = false;
		GetComponent<Collider>().enabled = true;
		
		Vector3 force = Camera.main.transform.forward + Camera.main.transform.up * 0.25f;
		m_rb.AddForce(force * throwForce);
		
		transform.SetParent(null);
	}

	private void CheckExplosionRadius()
	{
		List<Enemy> damagedEnemies = new List<Enemy>();

		foreach(Collider coll in Physics.OverlapSphere(transform.position, explosionRadius, grenadeLayerMask, QueryTriggerInteraction.Ignore))
		{
			float percentFromCenter = Vector3.Distance(coll.transform.position, transform.position) / explosionRadius;
			float damage = Mathf.Clamp01(1 - percentFromCenter) * maxDamage;

			// If collider is an enemy and they have not already been damaged
			Enemy enemy = coll.GetComponentInParent<Enemy>();
			
			if(enemy != null && !damagedEnemies.Contains(enemy))
			{
				enemy.health -= damage;

				// Set enemy to ragdoll
				Ragdoll enemyRd = enemy.GetComponent<Ragdoll>();
				enemyRd.RagdollOn = true;
				
				// Make them fly
				enemyRd.AddExplosionForce(explosionForce, transform.position, explosionRadius);

				damagedEnemies.Add(enemy);
			}

			else if(coll.TryGetComponent(out FPSController fpsController))
				fpsController.health -= damage;
		}
		
		damagedEnemies.Clear();
	}
}