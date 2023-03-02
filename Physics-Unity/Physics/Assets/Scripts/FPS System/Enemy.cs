using System.Collections;
using System.Collections.Generic;

using UnityEngine;
using UnityEngine.AI;

[RequireComponent(typeof(Ragdoll), typeof(NavMeshAgent))]
public class Enemy : MonoBehaviour
{
    public float health = 100;
    [SerializeField] private float damage = 10;
    [SerializeField, Tooltip("Degree/Second")] private float turnSpeed = 360;
    [SerializeField] private float attackRadius = 1;
    [SerializeField] private Transform playerCollider;

    public Collider HeadCollider => m_rd.animator.GetBoneTransform(HumanBodyBones.Head).GetComponent<Collider>();

    private Coroutine m_deathCR;
    private Ragdoll m_rd;
    private NavMeshAgent m_agent;
    private List<FPSController> m_players = new List<FPSController>();

    private void Start()
    {
        m_rd = GetComponent<Ragdoll>();

        m_agent = GetComponent<NavMeshAgent>();
        m_agent.speed = 0;
        m_agent.angularSpeed = turnSpeed;
        m_agent.stoppingDistance = attackRadius;

        foreach(FPSController player in FindObjectsOfType<FPSController>())
        {
            m_players.Add(player);
        }
    }
    
    private void Update()
    {
        // Move the player collider, only for walk type 2 (crawl-run)
        if(m_rd.animator.GetInteger("WalkType") != -1)
        {
            playerCollider.transform.localPosition = new Vector3(0, 0.25f, 0);
            playerCollider.transform.localEulerAngles = new Vector3(90, 0, 0);
        }
        
        // Set target to be nearest player, if not ragdolling
        SetTarget();
        
        // If dead, run death coroutine
        if(health <= 0)
        {
            m_deathCR ??= StartCoroutine(OnDeath());
            return;
        }
        
        AttackLogic();

        // Agent is off if ragdolling
        m_agent.enabled = !m_rd.RagdollOn;
        m_agent.speed = m_rd.animator.GetFloat("ZombieSpeed");

        // If ragdolling and movement is slow enough, un-ragdoll
        if(m_rd.RagdollOn && m_rd.TotalMovement < 1)
            m_rd.RagdollOn = false;
    }

    private void AttackLogic()
    {
        GetClosestPlayer(out FPSController closestPlayer, out float distance);
        
        // Player is in attack radius, start the attack
        if(distance <= attackRadius)
        {
            // Set start attack to true
            if (!m_rd.animator.GetBool("StartAttack"))
                m_rd.animator.SetBool("StartAttack", true);
        }
        
        // End of attack triggered
        if(m_rd.animator.GetBool("EndAttack"))
        {
            if (distance <= attackRadius) // player within radius, damage them
                closestPlayer.health -= damage;
            
            m_rd.animator.SetBool("EndAttack", false);
        }
    }

    private IEnumerator OnDeath()
    {
        m_rd.RagdollOn = true;
        m_agent.ResetPath();

        yield return new WaitForSeconds(3);
        
        Destroy(gameObject);
    }
    
    private void SetTarget()
    {
        if(!m_agent.enabled)
            return;

        if(!m_rd.RagdollOn)
        {
            GetClosestPlayer(out FPSController closestPlayer);
        
            if(closestPlayer != null)
            {
                m_agent.SetDestination(closestPlayer.transform.position);
            }
        }
        else m_agent.ResetPath();
    }

    private void GetClosestPlayer(out FPSController _closestPlayer, out float _distance)
    {
        _distance = float.MaxValue;
        _closestPlayer = null;

        foreach(FPSController player in m_players)
        {
            float playerDist = Vector3.Distance(player.transform.position, transform.position);

            if(playerDist < _distance)
            {
                _closestPlayer = player;
                _distance = playerDist;
            }
        }
    }

    private void GetClosestPlayer(out float _distance) => GetClosestPlayer(out FPSController closestPlayer, out _distance);

    private void GetClosestPlayer(out FPSController _closestPlayer) => GetClosestPlayer(out _closestPlayer, out float distance);
}
