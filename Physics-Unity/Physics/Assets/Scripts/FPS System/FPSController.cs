using System;

using UnityEngine;

using Random = UnityEngine.Random;

public class FPSController : MonoBehaviour
{
    public float health = 100;
    public GunType currentGun;
    
    private Transform m_camTransform;
    
    private float m_maxDelay;
    private float m_currentDelay;
    
    [SerializeField] private Grenade grenadePrefab;
    [SerializeField] private Transform grenadeHoldPt;
    [SerializeField] private int grenadeCount = 1;
    private Grenade m_grenade;

    private void Start()
    {
        m_camTransform = Camera.main.transform;
        m_maxDelay = 1f / currentGun.fireRate;
    }
    
    private void Update()
    {
        // Holding a grenade
        if(m_grenade != null)
            GrenadeLogic();

        // Not holding grenade, i.e. gun
        else ShootLogic();

        // Switch grenade out state
        if(Input.GetKeyDown(KeyCode.G))
            SwitchGrenadeState();

        // Increment the fire delay
        m_currentDelay += Time.deltaTime;
        
        // No health, player has died
        if (health <= 0)
            OnDeath();
    }

    private bool CanShoot()
    {
        bool canShoot = false;

        if(m_currentDelay >= m_maxDelay)
        {
            // Gun is automatic, so can shoot
            if (currentGun.automatic)
                canShoot = true;

            // Gun is not automatic, player has to re-click to shoot
            if (Input.GetMouseButtonDown(0))
                canShoot = true;

            // Only reset the delay if the player is about to shoot
            if(canShoot) m_currentDelay = 0;
        }

        return canShoot;
    }

    private void ShootLogic()
    {
        if(Input.GetMouseButton(0) && CanShoot())
        {
            Vector3 shotOffset = new Vector3(1, 1, 0) * Random.Range(-currentGun.bulletSpread, currentGun.bulletSpread);
            
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            // ray.direction = Quaternion.Euler(shotOffset) * ray.direction;

            // Checks if we are aiming at an object, using the random degree offset
            if(Physics.Raycast(ray, out RaycastHit hit, 500, -1, QueryTriggerInteraction.Ignore))
            {
                // Bullet hit an enemy
                Enemy enemy = hit.collider.GetComponentInParent<Enemy>();

                Debug.Log(hit.collider.name);
                
                if(enemy != null)
                {
                    float damage = hit.collider == enemy.HeadCollider ? 
                                       currentGun.headDamage : currentGun.bodyDamage;

                    enemy.DoDamage(damage);
                }
            }
        }
    }

    private void GrenadeLogic()
    {
        // Sets grenade to cook
        if(Input.GetMouseButtonDown(0))
            m_grenade.ShouldCook = true;

        // Throw the grenade
        if(Input.GetMouseButtonUp(0))
        {
            m_grenade.ThrowGrenade();
            m_grenade = null;

            grenadeCount--;
        }
    }

    private void SwitchGrenadeState()
    {
        // No grenade being held
        if(m_grenade == null)
        {
            // If there is grenade ammo, spawn a new grenade
            if(grenadeCount > 0)
            {
                m_grenade = Instantiate(grenadePrefab, grenadeHoldPt);
                m_grenade.gameObject.SetActive(true);
            
                // Set it to not collide until thrown
                m_grenade.GetComponent<Collider>().enabled = false;
            }
        }
        
        // If a grenade is currently being held, destroy it
        else
        {
            Destroy(m_grenade.gameObject);
            m_grenade = null;
        }
    }

    private void OnDeath()
    {
        
    }
}
