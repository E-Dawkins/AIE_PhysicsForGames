using UnityEngine;

public class Pendulum : MonoBehaviour
{
    [SerializeField] private Rigidbody pendulumEnd;

    private void Awake() => pendulumEnd.isKinematic = true;

    public void Release() => pendulumEnd.isKinematic = false;
}
