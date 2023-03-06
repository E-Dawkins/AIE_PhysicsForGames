using System;
using System.Collections;
using UnityEngine;

[RequireComponent(typeof(ConfigurableJoint))]
public class Crusher : MonoBehaviour
{
	[SerializeField] private Vector3 jointTargetPos;
	
	private ConfigurableJoint m_joint;

	private bool m_canCrush = true;

	private void Awake()
	{
		m_joint = GetComponent<ConfigurableJoint>();
	}

	public void StartCrush()
	{
		if (m_canCrush)
			StartCoroutine(Crush().GetEnumerator());
	}

	public IEnumerable Crush()
	{
		m_canCrush = false;
		
		m_joint.targetPosition = jointTargetPos;

		yield return new WaitForSeconds(2);
			
		m_joint.targetPosition = new Vector3();
			
		JointDrive zDrive = m_joint.zDrive;
		zDrive.positionSpring *= 0.5f;
		m_joint.zDrive = zDrive;

		yield return new WaitForSeconds(1);

		zDrive.positionSpring *= 2f;
		m_joint.zDrive = zDrive;

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