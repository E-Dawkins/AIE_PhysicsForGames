using UnityEngine;

public class Button : Interactable
{
	private void Update()
	{
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
		
		if(Physics.Raycast(ray, out RaycastHit hit, interactDistance))
		{
			if(hit.collider.gameObject == gameObject && Input.GetKey(KeyCode.E))
			{
				onActivation.Invoke();
			}
		}
	}
}