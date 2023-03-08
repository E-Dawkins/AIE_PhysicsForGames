using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Launcher : MonoBehaviour
{
	[SerializeField] private NavMeshObstacle navMeshObstacle;
	
	private List<Enemy> m_enemiesInArea = new List<Enemy>();

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
		Enemy enemy = _other.GetComponentInParent<Enemy>();
		
		if (enemy != null && !m_enemiesInArea.Contains(enemy))
			m_enemiesInArea.Add(enemy);
	}
	
	private void OnTriggerExit(Collider _other)
	{
		Enemy enemy = _other.GetComponentInParent<Enemy>();
		
		if (enemy != null && m_enemiesInArea.Contains(enemy))
			m_enemiesInArea.Remove(enemy);
	}

	public void StartLaunch(float _angle)
	{
		m_launchCR ??= StartCoroutine(Launch(_angle).GetEnumerator());
	}

	private IEnumerable Launch(float _angle)
	{
		GetComponent<Rigidbody>().isKinematic = false;
		navMeshObstacle.enabled = true;

		foreach(Enemy enemy in m_enemiesInArea)
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