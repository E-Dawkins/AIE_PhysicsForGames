using UnityEngine;

public class DeathBarrier : MonoBehaviour
{
	[SerializeField] private bool killEnemy;
	[SerializeField] private bool killPlayer;
	
	private void OnCollisionEnter(Collision _collision)
	{
		if (killEnemy)
		{
			Enemy enemy = _collision.gameObject.GetComponentInParent<Enemy>();

			if(enemy != null)
				enemy.health = -1;
		}

		if (killPlayer)
		{
			FPSController player = _collision.gameObject.GetComponentInParent<FPSController>();

			if(player != null)
				player.health = -1;
		}
	}
}