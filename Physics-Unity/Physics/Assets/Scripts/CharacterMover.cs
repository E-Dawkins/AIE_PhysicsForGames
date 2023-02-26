using UnityEngine;

[RequireComponent(typeof(CharacterController))]
public class CharacterMover : MonoBehaviour
{
	[SerializeField] private float movementSpeed = 10;
	[SerializeField] private float jumpHeight = 3;
	[SerializeField] private float crouchSpeedMulti = 0.25f;

	private CharacterController m_characterController;
	private Transform m_mainCam;
	private Animator m_animator;
	
	private Vector2 m_moveInput;
	private Vector3 m_velocity;
	private Vector3 m_hitDirection;
	
	private bool m_jumpInput;
	private bool m_crouchInput;
	private bool m_isGrounded;

	private void Awake()
	{
		m_characterController = GetComponent<CharacterController>();
		m_mainCam = Camera.main.transform;

		m_animator = GetComponentInChildren<Animator>();
	}

	private void Update()
	{
		m_moveInput.x = Input.GetAxis("Horizontal");
		m_moveInput.y = Input.GetAxis("Vertical");
		m_jumpInput = Input.GetButton("Jump");
		m_crouchInput = Input.GetButton("Crouch");

		m_animator.SetFloat("Forwards", m_moveInput.magnitude);
		m_animator.SetBool("Jump", !m_isGrounded);
		m_animator.SetBool("Crouch", m_crouchInput);
	}

	private void FixedUpdate()
	{
		// Find the horizontal unit vector facing forward from the camera
		Vector3 camForward = Vector3.ProjectOnPlane(m_mainCam.forward, Vector3.up);
		camForward.Normalize();

		// Use the camera's right vector, which is always horizontal
		Vector3 camRight = m_mainCam.right;
		
		// Get the movement delta using camera forward / right and the movement speed
		Vector3 delta = (m_moveInput.x * camRight + m_moveInput.y * camForward) * movementSpeed * (m_crouchInput ? crouchSpeedMulti : 1);

		// Face the player to the cameras forward
		transform.forward = camForward;

		// Only add movement when the player is grounded or inputs in the air
		if(m_isGrounded || m_moveInput.x != 0 || m_moveInput.y != 0)
		{
			m_velocity.x = delta.x;
			m_velocity.z = delta.z;
		}

		// Check for jump input, and we are grounded
		if(m_jumpInput && m_isGrounded)
		{
			float jumpVelocity = Mathf.Sqrt(2 * Physics.gravity.magnitude * jumpHeight);
			m_velocity.y = jumpVelocity;
		}
		
		// We've hit the ground from falling, so zero our velocity
		if(m_isGrounded && m_velocity.y < 0)
			m_velocity.y = 0;
		
		// Apply gravity after zeroing velocity so we register as grounded still
		m_velocity += Physics.gravity * Time.fixedDeltaTime;

		if(!m_isGrounded)
			m_hitDirection = Vector3.zero;

		// Slide objects off surfaces they're hanging onto
		if(m_moveInput.x == 0 && m_moveInput.y == 0)
		{
			Vector3 horizontalHitDirection = m_hitDirection;
			horizontalHitDirection.y = 0;

			float displacement = horizontalHitDirection.magnitude;

			if(displacement > 0)
			{
				Ray ray = new Ray(transform.position + m_characterController.center, -transform.up);

				// Only apply the sliding force if the player is standing off an edge, i.e. no ground under them
				if(!Physics.SphereCast(ray, m_characterController.radius * 0.2f, m_characterController.height * 0.5f, -1, QueryTriggerInteraction.Ignore))
				{
					m_velocity -= horizontalHitDirection / displacement;
				}
			}
		}

		m_characterController.Move(m_velocity * Time.deltaTime);
		m_isGrounded = m_characterController.isGrounded;
	}

	// Gets the direction that the character controller is colliding with
	private void OnControllerColliderHit(ControllerColliderHit _hit)
	{
		m_hitDirection = _hit.point - transform.position;
	}
}