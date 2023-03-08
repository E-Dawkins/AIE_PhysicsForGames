using System.Collections.Generic;
using UnityEngine;

public class PauseMenu : MonoBehaviour
{
    [SerializeField] private KeyCode pauseKey;
    [SerializeField] private GameObject pauseScreen;
    [SerializeField] private List<GameObject> otherUIElements = new List<GameObject>();

    private void Start()
    {
        pauseScreen.SetActive(false);
    }
    
    private void Update()
    {
        if(Input.GetKeyDown(pauseKey))
        {
            bool pauseScreenActive = pauseScreen.activeInHierarchy;
            
            pauseScreen.SetActive(!pauseScreenActive);
            
            foreach (GameObject go in otherUIElements)
                go.SetActive(pauseScreenActive);

            Time.timeScale = pauseScreenActive ? 1 : 0;

            Cursor.lockState = pauseScreenActive ? CursorLockMode.Locked : CursorLockMode.None;
            Cursor.visible = !pauseScreenActive;
        }
    }

    public void UnPause()
    {
        Time.timeScale = 1;
    }
}
