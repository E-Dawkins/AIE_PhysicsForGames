using System.Collections.Generic;
using UnityEngine;

public class Ragdoll : MonoBehaviour
{
    public Animator animator;
    [SerializeField] private List<Rigidbody> rigidbodies = new List<Rigidbody>();

    public bool RagdollOn
    {
        get => !animator.enabled;
        set
        {
            animator.enabled = !value;

            foreach(Rigidbody rb in rigidbodies)
                rb.isKinematic = !value;
            
            if (!value) AlignPosToHips();
        }
    }

    private Transform m_hipsBone;

    private void Awake()
    {
        m_hipsBone = animator.GetBoneTransform(HumanBodyBones.Hips);

        foreach(Rigidbody rb in rigidbodies)
        {
            rb.isKinematic = false;
            rb.detectCollisions = true;
        }
    }

    public void AddExplosionForce(float _forceAmount, Vector3 _position, float _radius)
    {
        foreach(Rigidbody rb in rigidbodies)
        {
            rb.AddExplosionForce(_forceAmount, _position, _radius, _radius * 0.25f, ForceMode.Impulse);
        }
    }

    public void AddForce(Vector3 _force, ForceMode _forceMode)
    {
        foreach(Rigidbody rb in rigidbodies)
        {
            rb.AddForce(_force, _forceMode);
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

    public float TotalMovement()
    {
        float movement = 0;
        
        foreach(Rigidbody rb in rigidbodies)
        {
            movement += rb.velocity.magnitude;
        }

        return movement;
    }
}