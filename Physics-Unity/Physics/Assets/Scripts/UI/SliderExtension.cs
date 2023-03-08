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
        float actualVal = PlayerPrefs.GetFloat(dataName);
        GetComponent<Slider>().value = actualVal;
        
        float percentVal = 100f * PlayerPrefs.GetFloat(dataName);
        sliderVisual.text = percentVal.ToString("n1");
    }

    public void OnSliderChanged(float _value)
    {
        PlayerPrefs.SetFloat(dataName, _value);
        PlayerPrefs.Save();

        float actualVal = 100f * _value;
        sliderVisual.text = actualVal.ToString("n1");
    }
}
