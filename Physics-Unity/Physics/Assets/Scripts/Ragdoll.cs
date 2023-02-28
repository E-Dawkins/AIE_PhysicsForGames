using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Animator))]
public class Ragdoll : MonoBehaviour
{
    [SerializeField] private List<Rigidbody> rigidbodies = new List<Rigidbody>();

    public bool RagdollOn
    {
        get => !m_animator.enabled;
        set
        {
            m_animator.enabled = !value;

            foreach(Rigidbody rb in rigidbodies)
                rb.isKinematic = !value;
            
            if (!value) AlignPosToHips();
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

    public Animator Animator => m_animator;

    private Transform m_hipsBone;
    private Animator m_animator;

    private void Awake()
    {
        m_animator = GetComponent<Animator>();

        m_hipsBone = m_animator.GetBoneTransform(HumanBodyBones.Hips);
        
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

    private void AlignPosToHips()
    {
        // Store the original hip position
        Vector3 originalHipPos = m_hipsBone.position;
        transform.position = m_hipsBone.position;

        // Setting the y-pos to the grounds height
        if(Physics.Raycast(transform.position, Vector3.down, out RaycastHit hit))
        {
            transform.position = new Vector3(transform.position.x, hit.point.y, transform.position.z);
        }

        m_hipsBone.position = originalHipPos;
    }
}
