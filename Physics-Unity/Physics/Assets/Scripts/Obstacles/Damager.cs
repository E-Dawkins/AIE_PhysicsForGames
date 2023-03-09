using UnityEngine;

public class Damager : MonoBehaviour
{
	[SerializeField] private bool damagePlayer;
	[SerializeField] private bool damageEnemy;

	[SerializeField] private float interval = 0.25f;
	[SerializeField] private float damage = 10;

	private float m_currentInterval;
	
	private void Update() => m_currentInterval -= Time.deltaTime;
	
	
	private void OnTriggerStay(Collider _collider)
	{
		if(m_currentInterval <= 0)
		{
			if(damageEnemy)
			{
				Zombie zombie = _collider.gameObject.GetComponentInParent<Zombie>();
				
				if(zombie != null)
					zombie.health -= damage;
			}

			if(damagePlayer)
			{
				FPSController player = _collider.gameObject.GetComponentInParent<FPSController>();
				
				if(player != null)
					player.health -= damage;
			}

			m_currentInterval = interval;
		}
	}
}