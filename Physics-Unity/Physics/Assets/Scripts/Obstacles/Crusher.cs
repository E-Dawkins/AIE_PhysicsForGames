using System;
using System.Collections;
using UnityEngine;

[RequireComponent(typeof(ConfigurableJoint))]
public class Crusher : MonoBehaviour
{
	[SerializeField] private Vector3 jointTargetPos;
	
	private ConfigurableJoint joint;

	private bool m_canCrush = true;

	private void Awake()
	{
		joint = GetComponent<ConfigurableJoint>();
	}

	public void StartCrush()
	{
		if (m_canCrush)
			StartCoroutine(Crush().GetEnumerator());
	}

	public IEnumerable Crush()
	{
		m_canCrush = false;
		
		joint.targetPosition = jointTargetPos;

		yield return new WaitForSeconds(2);
			
		joint.targetPosition = new Vector3();
			
		JointDrive zDrive = joint.zDrive;
		zDrive.positionSpring *= 0.5f;
		joint.zDrive = zDrive;

		yield return new WaitForSeconds(1);

		zDrive.positionSpring *= 2f;
		joint.zDrive = zDrive;

		m_canCrush = true;
	}

	private void OnCollisionEnter(Collision _collision)
	{
		Ragdoll rd = _collision.gameObject.GetComponentInParent<Ragdoll>();

		if(rd != null)
		{
			rd.RagdollOn = true;
		}
	}
}