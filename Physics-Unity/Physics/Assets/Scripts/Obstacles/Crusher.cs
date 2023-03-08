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
		m_joint = GetComponent<ConfigurableJoint>();
	}

	public void StartCrush()
	{
		m_crushCR ??= StartCoroutine(Crush().GetEnumerator());
	}

	public IEnumerable Crush()
	{
		m_joint.targetPosition = jointTargetPos;

		yield return new WaitForSeconds(2);
			
		m_joint.targetPosition = new Vector3();

		yield return new WaitForSeconds(1);

		m_crushCR = null;
	}

	private void OnCollisionEnter(Collision _collision)
	{
		Enemy enemy = _collision.gameObject.GetComponentInParent<Enemy>();

		if(enemy != null)
		{
			enemy.health = -1;
		}
	}
}