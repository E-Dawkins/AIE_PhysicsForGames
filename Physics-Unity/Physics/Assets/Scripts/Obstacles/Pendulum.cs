using UnityEngine;

public class Pendulum : MonoBehaviour
{
    [SerializeField] private Rigidbody pendulumEnd;

    // Set kinematic to true on awake
    private void Awake() => pendulumEnd.isKinematic = true;

    // When activated release the pendulum, by setting the end to non-kinematic
    public void Release() => pendulumEnd.isKinematic = false;
}
