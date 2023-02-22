using UnityEngine;

public class CameraController : MonoBehaviour
{
    [SerializeField] private Transform target;
    [SerializeField] private float heightOffset = 1.5f;
    [SerializeField] private float lookSpeed = 180;
    [SerializeField] private float relaxSpeed = 0.1f;
    
    [Header("Zooming")]
    [SerializeField] private float zoomSpeed = 1;
    [SerializeField] private float minZoomDist = 2;
    [SerializeField] private float maxZoomDist = 10;

    private float m_currentDistance;
    private float m_distanceBack;

    // Start is called before the first frame update
    private void Start()
    {
        m_distanceBack = minZoomDist;
        m_currentDistance = m_distanceBack;
    }

    // Update is called once per frame
    private void Update()
    {
        // Right click drag rotates the camera
        if(Input.GetMouseButton(1))
        {
            Vector3 angles = transform.eulerAngles;

            float dx = Input.GetAxis("Mouse Y");
            float dy = Input.GetAxis("Mouse X");
            
            // Look up and down by rotating around the X-axis
            angles.x = Mathf.Clamp(angles.x + dx * lookSpeed * Time.deltaTime, 0, 70);
            
            // Spin the camera sideways around target
            angles.y += dy * lookSpeed * Time.deltaTime;
            transform.eulerAngles = angles;
        }

        Ray ray = new Ray(GetTargetPosition(), -transform.forward);
        
        if(Physics.Raycast(ray, out RaycastHit hit, m_distanceBack, -1, QueryTriggerInteraction.Ignore))
        {
            // Snap the camera right in to where the collision happened
            m_currentDistance = hit.distance;
        }
        else
        {
            // Relax the camera back to the desired distance
            m_currentDistance = Mathf.Lerp(m_currentDistance, m_distanceBack, relaxSpeed);
        }

        // Zoom in / out with scroll wheel
        m_distanceBack = Mathf.Clamp(m_distanceBack - Input.GetAxis("Mouse ScrollWheel") * zoomSpeed, minZoomDist, maxZoomDist);
        
        // Look at target point
        transform.position = GetTargetPosition() - m_currentDistance * transform.forward;
    }

    Vector3 GetTargetPosition()
    {
        return target.position + heightOffset * Vector3.up;
    }
}
