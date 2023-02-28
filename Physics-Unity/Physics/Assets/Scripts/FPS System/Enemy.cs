using System.Collections;
using System.Collections.Generic;

using UnityEngine;
using UnityEngine.AI;

[RequireComponent(typeof(Ragdoll), typeof(NavMeshAgent))]
public class Enemy : MonoBehaviour
{
    [SerializeField] private float health = 100;
    [SerializeField] private Collider headCollider;
    [SerializeField, Tooltip("Degree/Second")] private float turnSpeed = 3;

    public Collider HeadCollider => headCollider;

    private Coroutine m_deathCR;
    private Ragdoll m_rd;
    private NavMeshAgent m_agent;
    private List<FPSController> m_players = new List<FPSController>();

    private void Start()
    {
        m_rd = GetComponent<Ragdoll>();

        m_agent = GetComponent<NavMeshAgent>();
        m_agent.speed = m_rd.Animator.GetFloat("ZombieSpeed");
        m_agent.angularSpeed = turnSpeed;
        
        foreach(FPSController player in FindObjectsOfType<FPSController>())
        {
            m_players.Add(player);
        }
    }
    
    private void Update()
    {
        if(health <= 0)
            return;

        // If ragdolling and movement is slow enough, un-ragdoll
        if(m_rd.RagdollOn && m_rd.TotalMovement < 1)
            m_rd.RagdollOn = false;
        
        // Set target to be nearest player, if not ragdolling
        SetTarget();

        // Agent is off if ragdolling
        m_agent.enabled = !m_rd.RagdollOn;
    }

    public void DoDamage(float _damage)
    {
        health -= _damage;
        
        // Spawn hit effect
        
        
        // If dead, run death coroutine
        if (health <= 0 && m_deathCR == null)
            m_deathCR = StartCoroutine(OnDeath());
    }

    private IEnumerator OnDeath()
    {
        m_rd.RagdollOn = true;

        yield return new WaitForSeconds(3);
        
        Destroy(gameObject);
    }

    private FPSController GetClosestPlayer()
    {
        float closestDistance = float.MaxValue;
        FPSController closestPlayer = null;

        foreach(FPSController player in m_players)
        {
            float playerDist = Vector3.Distance(player.transform.position, transform.position);

            if(playerDist < closestDistance)
            {
                closestPlayer = player;
                closestDistance = playerDist;
            }
        }

        return closestPlayer;
    }

    private void SetTarget()
    {
        if(!m_agent.enabled)
            return;

        if(!m_rd.RagdollOn)
        {
            FPSController closestPlayer = GetClosestPlayer();
        
            if(closestPlayer != null)
            {
                m_agent.SetDestination(closestPlayer.transform.position);
            }
        }
        else m_agent.ResetPath();
    }
}
