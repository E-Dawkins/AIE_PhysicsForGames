using TMPro;
using UnityEngine;
using UnityEngine.Events;

public class FPSController : MonoBehaviour
{
    public float health = 100;
    public int score;

    [SerializeField] private TextMeshProUGUI scoreText;
    
    [SerializeField] private Gun currentGun;
    [SerializeField] private Grenade grenadePrefab;

    [SerializeField] private Transform grenadeHoldPt;
    private Grenade m_grenade;

    [SerializeField] private GameObject interactUi;
    
    [SerializeField] private TextMeshProUGUI deathScoreText;
    [SerializeField] private UnityEvent onDeath;

    private void Awake() => interactUi.SetActive(false);

    private void Update()
    {
        // Toggle interact ui state
        ToggleUI();
        
        // If no grenade is held, gun must be active, and vice versa
        currentGun.gameObject.SetActive(m_grenade == null);

        // Holding a grenade
        if(m_grenade != null)
            GrenadeLogic();

        // Not holding grenade, i.e. gun
        else currentGun.GunLogic();

        // Switch grenade out state
        if(Input.GetKeyDown(KeyCode.G))
            SwitchGrenadeState();
        
        scoreText.text = $"Score : {score}";

        // No health, player has died
        if (health <= 0)
            OnDeath();
    }

    private void ToggleUI()
    {
        // Ray from screen center
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

        // We are looking at an interactable...
        if(Physics.Raycast(ray, out RaycastHit hit, Mathf.Infinity, -1, QueryTriggerInteraction.Ignore))
        {
            bool uiOn = hit.collider.TryGetComponent(out Interactable interactable) &&
                        hit.distance <= interactable.interactDistance;
            
            // ...so toggle the interact UI
            interactUi.SetActive(uiOn);
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
        }
    }

    private void SwitchGrenadeState()
    {
        // No grenade being held, spawn one
        if(m_grenade == null)
        {
            m_grenade = Instantiate(grenadePrefab, grenadeHoldPt);
            m_grenade.gameObject.SetActive(true);
            
            // Set it to not collide until thrown
            m_grenade.GetComponent<Collider>().enabled = false;
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
        // Turn the death screen UI on and run the death coroutine
        deathScoreText.text = $"Score : {score}";
        onDeath.Invoke();

        // Cursor set to visible
        Cursor.lockState = CursorLockMode.None;
        Cursor.visible = true;

        // And turn the controller script off
        enabled = false;
    }
}
