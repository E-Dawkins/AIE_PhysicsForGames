using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Launcher : MonoBehaviour
{
	public List<Enemy> enemiesInArea = new List<Enemy>();

	private HingeJoint m_joint;
	private Coroutine m_launchCR;

	private void Awake()
	{
		m_joint = GetComponent<HingeJoint>();
		GetComponent<Rigidbody>().isKinematic = true;
	}

	private void OnTriggerEnter(Collider _other)
	{
		Enemy enemy = _other.GetComponentInParent<Enemy>();
		
		if (enemy != null && !enemiesInArea.Contains(enemy))
			enemiesInArea.Add(enemy);
	}
	
	private void OnTriggerExit(Collider _other)
	{
		Enemy enemy = _other.GetComponentInParent<Enemy>();
		
		if (enemy != null && enemiesInArea.Contains(enemy))
			enemiesInArea.Remove(enemy);
	}

	public void StartLaunch(float _angle)
	{
		m_launchCR ??= StartCoroutine(Launch(_angle).GetEnumerator());
	}

	private IEnumerable Launch(float _angle)
	{
		GetComponent<Rigidbody>().isKinematic = false;

		foreach(Enemy enemy in enemiesInArea)
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
	}
}