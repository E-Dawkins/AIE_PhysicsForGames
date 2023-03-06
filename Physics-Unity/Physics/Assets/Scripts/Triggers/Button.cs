using UnityEngine;

public class Button : BaseTrigger
{
	[SerializeField] private GameObject ui;
	[SerializeField] private float clickDistance = 5;

	private void Awake() => ui.SetActive(false);

	private void Update()
	{
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

		bool canInteract = false;

		if(Physics.Raycast(ray, out RaycastHit hit, clickDistance))
		{
			// Player looking at us, and can run coroutine
			if(hit.collider.gameObject == gameObject)
				canInteract = true;

			// Player looking at us, and has pressed 'e'
			if(canInteract && Input.GetKeyDown(KeyCode.E))
			{
				Trigger();
			}
		}
		
		// UI on / off
		ui.SetActive(canInteract);
	}
}