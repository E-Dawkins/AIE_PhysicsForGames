using UnityEngine;

public class PendulumRelease : MonoBehaviour
{
    [SerializeField] private Rigidbody pendulumEnd;
    [SerializeField] private KeyCode releaseKey;

    private void Awake()
    {
        pendulumEnd.isKinematic = true;
    }
    
    private void Update()
    {
        if(Input.GetKeyDown(releaseKey))
            pendulumEnd.isKinematic = false;
    }
}
