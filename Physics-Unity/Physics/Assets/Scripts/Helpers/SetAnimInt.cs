using UnityEngine;

[RequireComponent(typeof(Animator))]
public class SetAnimInt : MonoBehaviour
{
	[SerializeField] private string intName;
	[SerializeField] private int intValue;

	private void Awake()
	{
		// Set animators int with its name and a value
		GetComponent<Animator>().SetInteger(intName, intValue);
	}
}
