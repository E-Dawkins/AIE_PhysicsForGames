using UnityEngine;

public class TriggerArea : Activator
{
	// Runs onActivation delegate when something enters the trigger area
	private void OnTriggerEnter(Collider _other) => onActivation.Invoke();
}