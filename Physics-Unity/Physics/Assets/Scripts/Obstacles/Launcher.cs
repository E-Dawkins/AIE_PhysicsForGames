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
		// Store reference to joint, set rigidbody to kinematic, and disable nav mesh obstacle
		m_joint = GetComponent<HingeJoint>();
		GetComponent<Rigidbody>().isKinematic = true;
		navMeshObstacle.enabled = false;
	}

	private void OnTriggerEnter(Collider _other)
	{
		// Check if zombie has entered the launch pad
		Zombie zombie = _other.GetComponentInParent<Zombie>();
		
		// Add zombie to list
		if (zombie != null && !m_enemiesInArea.Contains(zombie))
			m_enemiesInArea.Add(zombie);
	}
	
	private void OnTriggerExit(Collider _other)
	{
		// On exit remove the zombie from the list
		Zombie zombie = _other.GetComponentInParent<Zombie>();
		
		if (zombie != null && m_enemiesInArea.Contains(zombie))
			m_enemiesInArea.Remove(zombie);
	}

	public void StartLaunch(float _angle)
	{
		// Run the launch coroutine only if it is not already running
		m_launchCR ??= StartCoroutine(Launch(_angle).GetEnumerator());
	}

	private IEnumerable Launch(float _angle)
	{
		// Rigidbody not kinematic and enable nav mesh obstacle
		GetComponent<Rigidbody>().isKinematic = false;
		navMeshObstacle.enabled = true;

		// Foreach zombie on launch pad, ragdoll them
		foreach(Zombie enemy in m_enemiesInArea)
		{
			enemy.Ragdoll.RagdollOn = true;
			enemy.Agent.enabled = false;
		}
		
		// Logic to make launch pad launch, using the spring of the joint
		JointSpring spring = m_joint.spring;
		float startAngle = spring.targetPosition;

		spring.targetPosition = _angle;
		m_joint.spring = spring;

		yield return new WaitForSeconds(2);

		spring.targetPosition = startAngle;
		m_joint.spring = spring;

		yield return new WaitForSeconds(1);

		m_launchCR = null;
		
		// Rigidbody kinematic and disable nav mesh obstacle
		GetComponent<Rigidbody>().isKinematic = true;
		navMeshObstacle.enabled = false;
	}
}