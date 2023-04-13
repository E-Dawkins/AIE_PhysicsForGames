using UnityEngine;

public class CameraController : MonoBehaviour
{
    [SerializeField] private Transform target;
    [SerializeField] private Vector3 camOffset = new Vector3(0, 1.7f, 0.05f);
    [SerializeField] private float maxLookSpeed = 270;
    [SerializeField] private string sensDataName = "sensitivity";

    private float m_xRot;

    private void Start()
    {
        // Set cursor to be locked to screen center and invisible
        Cursor.lockState = CursorLockMode.Locked;
        Cursor.visible = false;
    }

    private void Update()
    {
        float actualLookSpeed = PlayerPrefs.GetFloat(sensDataName, 0.5f) * maxLookSpeed;

        // Rotate the camera with the mouse movement
        Vector3 angles = transform.eulerAngles;
        
        float dx = Input.GetAxis("Mouse Y") * actualLookSpeed * Time.deltaTime;
        float dy = Input.GetAxis("Mouse X") * actualLookSpeed * Time.deltaTime;
            
        // Look up and down by rotating around the X-axis
        m_xRot = Mathf.Clamp(m_xRot - dx, -70f, 55f);
        angles.x = m_xRot;

        // Spin the camera on the Y-axis
        angles.y += dy * actualLookSpeed * Time.deltaTime;
        transform.eulerAngles = angles;

        // Move camera with target
        transform.position = target.position + ComputeOffset();
    }

    // Helper function to compute the offset of the camera, just looks cleaner
    private Vector3 ComputeOffset()
    {
        return camOffset.x * target.right + camOffset.y * target.up + camOffset.z * target.forward;
    }

    // Draw sphere where camera will be in-game, for editor use
    private void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.magenta;
        Gizmos.DrawSphere(target.position + ComputeOffset(), 0.03f);
    }
}
