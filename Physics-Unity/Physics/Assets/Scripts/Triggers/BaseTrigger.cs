using UnityEngine;
using UnityEngine.Events;

public class BaseTrigger : MonoBehaviour
{
	public UnityEvent onTrigger;

	protected void Trigger()
	{
		onTrigger.Invoke();
	}
}