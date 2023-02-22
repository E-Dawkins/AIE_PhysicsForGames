using UnityEngine;

public class RagdollTrigger : MonoBehaviour
{
    private void OnTriggerEnter(Collider _other)
    {
        Ragdoll ragdoll = _other.GetComponentInParent<Ragdoll>();

        if(ragdoll != null)
            ragdoll.ragdollOn = true;
    }
}
