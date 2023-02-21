using System;

using UnityEngine;

[RequireComponent(typeof(CharacterController))]
public class CharacterMover : MonoBehaviour
{
	[SerializeField] private float movementSpeed = 10;
	[SerializeField] private float jumpHeight = 3;
	[SerializeField] private float rotateSpeed = 0.1f;

	private CharacterController _characterController;
	private Transform _mainCam;
	private Animator _animator;
	
	private Vector2 _moveInput;
	private Vector3 _velocity;
	private Vector3 _hitDirection;
	
	private bool _jumpInput;
	private bool _isGrounded;


	// Start is called before the first frame update
	private void Awake()
	{
		_characterController = GetComponent<CharacterController>();
		_mainCam = Camera.main.transform;

		_animator = GetComponentInChildren<Animator>();
	}

	// Update is called once per frame
	private void Update()
	{
		_moveInput.x = Input.GetAxis("Horizontal");
		_moveInput.y = Input.GetAxis("Vertical");
		_jumpInput = Input.GetButton("Jump");

		_animator.SetFloat("Forwards", _moveInput.magnitude);
		_animator.SetBool("Jump", !_isGrounded);
	}

	private void FixedUpdate()
	{
		// Find the horizontal unit vector facing forward from the camera
		Vector3 camForward = Vector3.ProjectOnPlane(_mainCam.forward, Vector3.up);
		camForward.Normalize();

		// Use the camera's right vector, which is always horizontal
		Vector3 camRight = _mainCam.right;
		
		// Get the movement delta using camera forward / right and the movement speed
		Vector3 delta = (_moveInput.x * camRight + _moveInput.y * camForward) * movementSpeed;

		// Face the player to the movement direction
		transform.forward = Vector3.Lerp(transform.forward, delta, rotateSpeed);

		// Only add movement when the player is grounded or inputs in the air
		if(_isGrounded || _moveInput.x != 0 || _moveInput.y != 0)
		{
			_velocity.x = delta.x;
			_velocity.z = delta.z;
		}

		// Check for jump input, and we are grounded
		if(_jumpInput && _isGrounded)
		{
			float jumpVelocity = Mathf.Sqrt(2 * Physics.gravity.magnitude * jumpHeight);
			_velocity.y = jumpVelocity;
		}
		
		// We've hit the ground from falling, so zero our velocity
		if(_isGrounded && _velocity.y < 0)
			_velocity.y = 0;
		
		// Apply gravity after zeroing velocity so we register as grounded still
		_velocity += Physics.gravity * Time.fixedDeltaTime;

		if(!_isGrounded)
			_hitDirection = Vector3.zero;

		// Slide objects off surfaces they're hanging onto
		if(_moveInput.x == 0 && _moveInput.y == 0)
		{
			Vector3 horizontalHitDirection = _hitDirection;
			horizontalHitDirection.y = 0;

			float displacement = horizontalHitDirection.magnitude;
			
			if(displacement > 0)
				_velocity -= horizontalHitDirection / displacement;
		}

		_characterController.Move(_velocity * Time.deltaTime);
		_isGrounded = _characterController.isGrounded;
	}

	private void OnControllerColliderHit(ControllerColliderHit _hit)
	{
		_hitDirection = _hit.point - transform.position;
	}
}