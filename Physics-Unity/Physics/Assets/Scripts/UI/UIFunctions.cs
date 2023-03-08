using UnityEngine;
using UnityEngine.SceneManagement;

public class UIFunctions : MonoBehaviour
{
    // Loads the scene with name '_sceneName'
    public void LoadScene(string _sceneName) => SceneManager.LoadScene(_sceneName);
    
    // Loads the scene asynchronously with name '_sceneName'
    public void LoadSceneAsync(string _sceneName) => SceneManager.LoadSceneAsync(_sceneName);

    public void Quit() // quits the game, or stops the editor playing
    {
        #if UNITY_EDITOR
			    UnityEditor.EditorApplication.isPlaying = false;
        #else
            Application.Quit();
        #endif
    }
}
