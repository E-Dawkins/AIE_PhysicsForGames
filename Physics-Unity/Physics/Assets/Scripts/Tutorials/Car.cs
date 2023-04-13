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
		// Store a list of hinge joints in car object
		foreach(HingeJoint joint in carObject.GetComponents<HingeJoint>())
		{
			joints.Add(joint);
		}
	}

	private void Update()
	{
		// Target pos and target speed set by player input
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
			// If joint use spring on, set its target position
			if(joint.useSpring)
			{
				JointSpring spring = joint.spring;
				spring.targetPosition = targetPos;
				joint.spring = spring;
			}

			// If joint use motor on, set its target velocity
			if(joint.useMotor)
			{
				JointMotor motor = joint.motor;
				motor.targetVelocity = targetSpeed;
				joint.motor = motor;
			}
		}

		// Foreach extra pivot, set its target position
		foreach(HingeJoint joint in extraPivots)
		{
			JointSpring spring = joint.spring;
			spring.targetPosition = targetPos;
			joint.spring = spring;
		}

		// Foreach extra joint, set its target velocity and force
		foreach(HingeJoint joint in extraMotors)
		{
			JointMotor motor = joint.motor;
			motor.targetVelocity = targetSpeed;
			motor.force = targetForce;
			joint.motor = motor;
		}
	}
}