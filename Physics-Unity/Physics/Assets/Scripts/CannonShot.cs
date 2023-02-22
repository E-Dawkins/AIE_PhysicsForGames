using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class CannonShot : MonoBehaviour
{
    [SerializeField] private float fireForce = 400;
    
    private bool m_canFire = true;
    private Rigidbody m_rb;

    // Start is called before the first frame update
    private void Awake()
    {
        m_rb = GetComponent<Rigidbody>();
        m_rb.isKinematic = true;
    }

    // Update is called once per frame
    private void Update()
    {
        if(Input.anyKeyDown && m_canFire)
        {
            m_rb.isKinematic = false;
            m_rb.AddForce(transform.forward * fireForce);
            m_canFire = false;
        }

        if(m_rb.IsSleeping())
            m_canFire = true;
    }
}
