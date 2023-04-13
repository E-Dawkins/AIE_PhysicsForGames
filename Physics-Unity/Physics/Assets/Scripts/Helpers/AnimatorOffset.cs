using UnityEngine;
using Random = UnityEngine.Random;

[RequireComponent(typeof(Animator))]
public class AnimatorOffset : MonoBehaviour
{
    [SerializeField] private string stateName;
    
    private Animator m_animator;

    // Store reference to animator
    private void Awake() => m_animator = GetComponent<Animator>();

    // Offset the state by a random amount
    private void Start() => m_animator.Play(stateName, 0, Random.Range(0f, 1f));
}
