using UnityEngine;

public class Button : Interactable
{
	private void Update()
	{
		// Ray from camera position to mouse position in world space
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
		
		if(Physics.Raycast(ray, out RaycastHit hit, interactDistance))
		{
			// Is the button being hovered over and 'E' is pressed?
			if(hit.collider.gameObject == gameObject && Input.GetKey(KeyCode.E))
			{
				// Run onActivation delegate
				onActivation.Invoke();
			}
		}
	}
}