using System.Collections;

using UnityEngine;

[RequireComponent(typeof(Ragdoll))]
public class Enemy : MonoBehaviour
{
    [SerializeField] private float health = 100;
    [SerializeField] private Collider headCollider;

    public Collider HeadCollider => headCollider;

    private Ragdoll rd;
    
    private void Awake()
    {
        rd = GetComponent<Ragdoll>();
    }
    
    private void Update()
    {
        if(health <= 0)
            return;

        // If ragdolling and movement is slow enough, un-ragdoll
        if(rd.RagdollOn && rd.TotalMovement < 1)
            rd.RagdollOn = false;
    }

    public void DoDamage(float _damage)
    {
        health -= _damage;
        
        // Spawn hit effect
        
        
        // If dead, run death coroutine
        if (health <= 0)
            StartCoroutine(OnDeath());
    }

    private IEnumerator OnDeath()
    {
        rd.RagdollOn = true;

        yield return new WaitForSeconds(3);
        
        Destroy(gameObject);
    }
}
