using System;
using System.Collections.Generic;

using UnityEngine;

public class Car : MonoBehaviour
{
	[SerializeField] private GameObject carObject;
	[SerializeField] private List<HingeJoint> extraPivots = new List<HingeJoint>();
	[SerializeField] private List<HingeJoint> extraMotors = new List<HingeJoint>();

	[SerializeField] private float leftTurn = 10;
	[SerializeField] private float rightTurn = -10;
	[SerializeField] private float forwardVelocity = -100;
	[SerializeField] private float backwardVelocity = 100;
	[SerializeField] private float targetForce = 100;

	private List<HingeJoint> joints = new List<HingeJoint>();

	private void Awake()
	{
		foreach(HingeJoint joint in carObject.GetComponents<HingeJoint>())
		{
			joints.Add(joint);
		}
	}

	private void Update()
	{
		float targetPos = 0;
		float targetSpeed = 0;

		if(Input.GetKey(KeyCode.A))
			targetPos = leftTurn;

		if(Input.GetKey(KeyCode.D))
			targetPos = rightTurn;

		if(Input.GetKey(KeyCode.W))
			targetSpeed = forwardVelocity;

		if(Input.GetKey(KeyCode.S))
			targetSpeed = backwardVelocity;

		foreach (HingeJoint joint in joints)
		{
			if(joint.useSpring)
			{
				JointSpring spring = joint.spring;
				spring.targetPosition = targetPos;
				joint.spring = spring;
			}

			if(joint.useMotor)
			{
				JointMotor motor = joint.motor;
				motor.targetVelocity = targetSpeed;
				joint.motor = motor;
			}
		}

		foreach(HingeJoint joint in extraPivots)
		{
			JointSpring spring = joint.spring;
			spring.targetPosition = targetPos;
			joint.spring = spring;
		}

		foreach(HingeJoint joint in extraMotors)
		{
			JointMotor motor = joint.motor;
			motor.targetVelocity = targetSpeed;
			motor.force = targetForce;
			joint.motor = motor;
		}
	}
}