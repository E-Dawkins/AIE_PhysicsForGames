using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class PhysicsObject : MonoBehaviour
{
    [SerializeField] private Material awakeMat;
    [SerializeField] private Material asleepMat;

    private Rigidbody m_rb = null;
    private bool m_isSleeping;

    // Start is called before the first frame update
    private void Start()
    {
        // store reference to rigidbody component
        m_rb = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    private void FixedUpdate()
    {
        // If rigidbody is asleep set renderer material to asleep material
        if(m_rb.IsSleeping() && m_isSleeping && asleepMat != null)
        {
            m_isSleeping = false;
            GetComponent<MeshRenderer>().material = asleepMat;
        }

        // If rigidbody is not sleeping, set renderer to awake material
        if(!m_rb.IsSleeping() && !m_isSleeping && awakeMat != null)
        {
            m_isSleeping = true;
            GetComponent<MeshRenderer>().material = awakeMat;
        }
    }
}
