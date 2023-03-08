using UnityEngine;

[RequireComponent(typeof(Animator))]
public class SetAnimInt : MonoBehaviour
{
	[SerializeField] private string intName;
	[SerializeField] private int intValue;

	private void Awake()
	{
		GetComponent<Animator>().SetInteger(intName, intValue);
	}
}
