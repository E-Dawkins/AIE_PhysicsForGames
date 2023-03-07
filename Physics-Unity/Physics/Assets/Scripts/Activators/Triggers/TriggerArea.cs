using UnityEngine;
using UnityEngine.Events;

public class TriggerArea : Activator
{
	public UnityEvent onDeactivation;

	private int m_totalInTrigger;
	
	private void OnTriggerEnter(Collider _other)
	{
		if (m_totalInTrigger == 0)
			onActivation.Invoke();

		m_totalInTrigger++;
	}

	private void OnTriggerExit(Collider _other)
	{
		m_totalInTrigger--;
		
		if (m_totalInTrigger == 0)
			onDeactivation.Invoke();
	}
}