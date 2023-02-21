using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    [SerializeField] private Transform target;
    [SerializeField] private float heightOffset = 1.5f;
    [SerializeField, Tooltip("Degrees per second")] 
    private float lookSpeed = 180;
    [SerializeField, Tooltip("How fast the camera relaxes to zoom distance")]
    private float relaxSpeed = 0.1f;
    
    [Header("Zooming")]
    [SerializeField] private float zoomSpeed = 1;
    [SerializeField] private float minZoomDist = 2;
    [SerializeField] private float maxZoomDist = 10;

    private float _currentDistance;
    private float _distanceBack;

    // Start is called before the first frame update
    private void Start()
    {
        _distanceBack = minZoomDist;
        _currentDistance = _distanceBack;
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

        if(Physics.Raycast(GetTargetPosition(), -transform.forward, out RaycastHit hit, _distanceBack))
        {
            // Snap the camera right in to where the collision happened
            _currentDistance = hit.distance;
        }
        else
        {
            // Relax the camera back to the desired distance
            _currentDistance = Mathf.Lerp(_currentDistance, _distanceBack, relaxSpeed);
        }

        // Zoom in / out with scroll wheel
        _distanceBack = Mathf.Clamp(_distanceBack - Input.GetAxis("Mouse ScrollWheel") * zoomSpeed, minZoomDist, maxZoomDist);
        
        // Look at target point
        transform.position = GetTargetPosition() - _currentDistance * transform.forward;
    }

    Vector3 GetTargetPosition()
    {
        return target.position + heightOffset * Vector3.up;
    }
}
