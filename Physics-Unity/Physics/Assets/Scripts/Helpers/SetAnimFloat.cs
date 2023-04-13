using UnityEngine;

[RequireComponent(typeof(Animator))]
public class SetAnimFloat : MonoBehaviour
{
    [SerializeField] private string floatName;
    [SerializeField] private float floatValue;

    private void Awake()
    {
        // Sets animators float using its name and a value
        GetComponent<Animator>().SetFloat(floatName, floatValue);
    }
}
