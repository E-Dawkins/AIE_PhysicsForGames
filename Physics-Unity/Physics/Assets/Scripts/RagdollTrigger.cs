using UnityEngine;

public class RagdollTrigger : MonoBehaviour
{
    [SerializeField] private bool applyForce;
    [SerializeField] private float forceMag;

    private void OnTriggerEnter(Collider _other)
    {
        Ragdoll ragdoll = _other.GetComponentInParent<Ragdoll>();

        if(ragdoll != null)
        {
            ragdoll.RagdollOn = true;

            if(applyForce)
            {
                // Apply force to ragdoll, in direction from THIS -> RAGDOLL
                Vector3 dir = (ragdoll.transform.position - transform.position).normalized;
            
                // Apply the force to every ragdoll rigidbody, with a velocity change
                ragdoll.AddForce(dir * forceMag, ForceMode.VelocityChange);
            }
        }
    }
}
