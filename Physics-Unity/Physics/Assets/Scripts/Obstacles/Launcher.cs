using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Launcher : MonoBehaviour
{
	[SerializeField] private NavMeshObstacle navMeshObstacle;
	
	private List<Zombie> m_enemiesInArea = new List<Zombie>();

	private HingeJoint m_joint;
	private Coroutine m_launchCR;

	private void Awake()
	{
		m_joint = GetComponent<HingeJoint>();
		GetComponent<Rigidbody>().isKinematic = true;
		navMeshObstacle.enabled = false;
	}

	private void OnTriggerEnter(Collider _other)
	{
		Zombie zombie = _other.GetComponentInParent<Zombie>();
		
		if (zombie != null && !m_enemiesInArea.Contains(zombie))
			m_enemiesInArea.Add(zombie);
	}
	
	private void OnTriggerExit(Collider _other)
	{
		Zombie zombie = _other.GetComponentInParent<Zombie>();
		
		if (zombie != null && m_enemiesInArea.Contains(zombie))
			m_enemiesInArea.Remove(zombie);
	}

	public void StartLaunch(float _angle)
	{
		m_launchCR ??= StartCoroutine(Launch(_angle).GetEnumerator());
	}

	private IEnumerable Launch(float _angle)
	{
		GetComponent<Rigidbody>().isKinematic = false;
		navMeshObstacle.enabled = true;

		foreach(Zombie enemy in m_enemiesInArea)
		{
			enemy.Ragdoll.RagdollOn = true;
			enemy.Agent.enabled = false;
		}
		
		JointSpring spring = m_joint.spring;
		float startAngle = spring.targetPosition;

		spring.targetPosition = _angle;
		m_joint.spring = spring;

		yield return new WaitForSeconds(2);

		spring.targetPosition = startAngle;
		m_joint.spring = spring;

		yield return new WaitForSeconds(1);

		m_launchCR = null;
		
		GetComponent<Rigidbody>().isKinematic = true;
		navMeshObstacle.enabled = false;
	}
}