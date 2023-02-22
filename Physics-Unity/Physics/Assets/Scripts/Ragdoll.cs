using System.Collections.Generic;

using UnityEngine;

[RequireComponent(typeof(Animator))]
public class Ragdoll : MonoBehaviour
{
    [SerializeField] private List<Rigidbody> rigidbodies = new List<Rigidbody>();

    public bool ragdollOn
    {
        get => !m_animator.enabled;
        set
        {
            m_animator.enabled = !value;
            
            foreach(Rigidbody rb in rigidbodies)
                rb.isKinematic = !value;
        }
    }

    private Animator m_animator;

    // Start is called before the first frame update
    private void Start()
    {
        m_animator = GetComponent<Animator>();
        
        foreach(Rigidbody rb in rigidbodies)
            rb.isKinematic = false;
    }
}
