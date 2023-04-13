using System.Collections.Generic;
using UnityEngine;

public class PauseMenu : MonoBehaviour
{
    [SerializeField] private KeyCode pauseKey;
    [SerializeField] private GameObject pauseScreen;
    [SerializeField] private List<GameObject> otherUIElements = new List<GameObject>();

    private void Start()
    {
        // Set pause screen to inactive by default
        pauseScreen.SetActive(false);
    }
    
    private void Update()
    {
        // If pause key is pressed...
        if(Input.GetKeyDown(pauseKey))
        {
            bool pauseScreenActive = pauseScreen.activeInHierarchy;
            
            // ...toggle pause screen active state
            pauseScreen.SetActive(!pauseScreenActive);
            
            // Foreach object in other ui elements list, set them active
            foreach (GameObject go in otherUIElements)
                go.SetActive(pauseScreenActive);

            // Toggle time scale, and cursor state
            Time.timeScale = pauseScreenActive ? 1 : 0;

            Cursor.lockState = pauseScreenActive ? CursorLockMode.Locked : CursorLockMode.None;
            Cursor.visible = !pauseScreenActive;
        }
    }

    // Helper function to set time scale
    public void SetTimeScale(float _scale) => Time.timeScale = _scale;
}
