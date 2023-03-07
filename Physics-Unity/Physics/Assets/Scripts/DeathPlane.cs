using UnityEngine;

public class DeathPlane : MonoBehaviour
{
	[SerializeField] private Vector3 playerResetPos;
	
	private void OnCollisionEnter(Collision _collision)
	{
		if(_collision.gameObject.TryGetComponent(out FPSController player))
			player.transform.position = playerResetPos;

		if(_collision.gameObject.TryGetComponent(out Enemy enemy))
			Destroy(enemy);
	}
}