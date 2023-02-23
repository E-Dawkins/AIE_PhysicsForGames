using UnityEngine;

[CreateAssetMenu(fileName = "Gun", menuName = "ScriptableObjects/GunType", order = 1)]
public class GunType : ScriptableObject
{
    public float damage = 10;
    [Tooltip("Bullets / Second")]
    public float fireRate = 3;
    [Tooltip("Max degree offset")]
    public float bulletSpread = 2.5f;
    [Tooltip("In seconds")]
    public float reloadSpeed = 0.8f;
    [Tooltip("Can the shoot button be held?")]
    public bool automatic;
}
