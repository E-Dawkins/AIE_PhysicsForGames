using UnityEditor.SceneManagement;
using UnityEngine.SceneManagement;
using UnityEngine;

[ExecuteInEditMode]
public class SetAllRbsDynamic : MonoBehaviour
{
	[SerializeField] private bool run;

	private void Update()
	{
		if(run)
		{
			foreach(Rigidbody rb in GetComponentsInChildren<Rigidbody>())
			{
				rb.interpolation = RigidbodyInterpolation.Interpolate;
				rb.collisionDetectionMode = CollisionDetectionMode.ContinuousDynamic;
			}

			run = !run;
			
			EditorSceneManager.MarkSceneDirty(SceneManager.GetActiveScene());
		}
	}
}