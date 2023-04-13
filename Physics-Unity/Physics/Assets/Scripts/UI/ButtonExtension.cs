using UnityEngine;
using UnityEngine.Events;
using UnityEngine.EventSystems;

[RequireComponent(typeof(UnityEngine.UI.Button))]
public class ButtonExtension : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
	[SerializeField] private UnityEvent onHover;
	[SerializeField] private UnityEvent onUnHover;

	public void OnPointerEnter(PointerEventData _eventData)
	{
		// On hover, invoke onHover delegate
		onHover.Invoke();
	}

	public void OnPointerExit(PointerEventData _eventData)
	{
		// On un-hover, invoke onUnHover delegate
		onUnHover.Invoke();
	}
}