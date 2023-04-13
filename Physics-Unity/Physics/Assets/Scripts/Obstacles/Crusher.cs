using System.Collections;
using UnityEngine;

[RequireComponent(typeof(ConfigurableJoint))]
public class Crusher : MonoBehaviour
{
	[SerializeField] private Vector3 jointTargetPos;
	
	private ConfigurableJoint m_joint;

	private Coroutine m_crushCR;

	private void Awake()
	{
		// Store reference to joint component
		m_joint = GetComponent<ConfigurableJoint>();
	}

	public void StartCrush()
	{
		// Run the crush coroutine if it is not already playing
		m_crushCR ??= StartCoroutine(Crush().GetEnumerator());
	}

	public IEnumerable Crush()
	{
		// Set joint target position
		m_joint.targetPosition = jointTargetPos;

		// Wait...
		yield return new WaitForSeconds(2);
			
		// Reset joint target position
		m_joint.targetPosition = new Vector3();

		// Wait...
		yield return new WaitForSeconds(1);

		// Set crush coroutine to null
		m_crushCR = null;
	}

	private void OnCollisionEnter(Collision _collision)
	{
		// Check if zombie has collided with crusher, and kill them
		Zombie zombie = _collision.gameObject.GetComponentInParent<Zombie>();

		if(zombie != null)
		{
			zombie.health = -1;
		}
	}
}