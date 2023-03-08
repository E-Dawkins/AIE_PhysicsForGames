using UnityEngine;

[RequireComponent(typeof(Animator))]
public class SetAnimFloat : MonoBehaviour
{
    [SerializeField] private string floatName;
    [SerializeField] private float floatValue;

    private void Awake()
    {
        GetComponent<Animator>().SetFloat(floatName, floatValue);
    }
}
