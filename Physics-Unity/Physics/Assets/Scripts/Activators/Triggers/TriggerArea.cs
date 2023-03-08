using UnityEngine;

public class TriggerArea : Activator
{
	private void OnTriggerEnter(Collider _other) => onActivation.Invoke();
}