using UnityEngine;

public class FPSController : MonoBehaviour
{
    public GunType currentGun;

    [SerializeField] private GameObject DebugHitPoint;

    private Transform m_camTransform;
    private float m_maxDelay;
    private float m_currentDelay;
    private bool m_canShoot;
    
    private void Start()
    {
        m_camTransform = Camera.main.transform;
        m_maxDelay = 1f / currentGun.fireRate;
        m_canShoot = true;
    }
    
    private void Update()
    {
        // Shooting logic
        if(Input.GetMouseButton(0) && m_canShoot)
        {
            Vector3 shotOffset = new Vector3(1, 1, 0) * Random.Range(-currentGun.bulletSpread, currentGun.bulletSpread);

            Ray ray = new Ray(m_camTransform.position, m_camTransform.forward);
            ray.direction = Quaternion.Euler(shotOffset) * ray.direction;
            
            // Checks if we are aiming at an object, using the random degree offset
            if(Physics.Raycast(ray, out RaycastHit hit, Mathf.Infinity, -1, QueryTriggerInteraction.Ignore))
            {
                Instantiate(DebugHitPoint, hit.point, Quaternion.identity);
            }

            m_canShoot = false;
        }

        m_currentDelay += Time.deltaTime;

        // Checks if the player can shoot
        if(m_currentDelay >= m_maxDelay)
        {
            // Gun is automatic, so can shoot
            if (currentGun.automatic)
                m_canShoot = true;

            // Gun is not automatic, player has to re-click to shoot
            else if (Input.GetMouseButtonDown(0))
                m_canShoot = true;

            // Only reset the delay if the player is about to shoot
            if(m_canShoot) m_currentDelay = 0;
        }
    }
}
