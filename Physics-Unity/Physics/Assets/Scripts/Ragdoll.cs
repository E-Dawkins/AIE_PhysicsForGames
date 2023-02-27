using System.Collections.Generic;

using UnityEngine;

[RequireComponent(typeof(Animator))]
public class Ragdoll : MonoBehaviour
{
    [SerializeField] private List<Rigidbody> rigidbodies = new List<Rigidbody>();
    public Rigidbody rdCenterRb;

    public bool RagdollOn
    {
        get => !m_animator.enabled;
        set
        {
            m_animator.enabled = !value;
            
            foreach(Rigidbody rb in rigidbodies)
                rb.isKinematic = !value;
        }
    }

    public float TotalMovement
    {
        get
        {
            float temp = 0;

            foreach(Rigidbody rb in rigidbodies)
                temp += rb.velocity.magnitude;

            return temp;
        }
    }

    private Animator m_animator;
    
    private void Start()
    {
        m_animator = GetComponent<Animator>();
        
        foreach(Rigidbody rb in rigidbodies)
            rb.isKinematic = false;
    }

    public void AddExplosionForce(float _forceAmount, Vector3 _position, float _radius)
    {
        foreach(Rigidbody rb in rigidbodies)
        {
            rb.AddExplosionForce(_forceAmount, _position, _radius, _radius * 0.25f, ForceMode.Impulse);
        }
    }
}
