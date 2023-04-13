using TMPro;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Slider))]
public class SliderExtension : MonoBehaviour
{
    [SerializeField] private string dataName;
    [SerializeField] private TextMeshProUGUI sliderVisual;

    private void Awake()
    {
        // Set slider value to stored value
        float actualVal = PlayerPrefs.GetFloat(dataName);
        GetComponent<Slider>().value = actualVal;
        
        // Set slider text to percent value of the slider
        float percentVal = 100f * PlayerPrefs.GetFloat(dataName);
        sliderVisual.text = percentVal.ToString("n1");
    }

    public void OnSliderChanged(float _value)
    {
        // On slider changed store the new value
        PlayerPrefs.SetFloat(dataName, _value);
        PlayerPrefs.Save();

        // Change slider text to new percent value of the slider
        float actualVal = 100f * _value;
        sliderVisual.text = actualVal.ToString("n1");
    }
}
