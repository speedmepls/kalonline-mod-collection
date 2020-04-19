void __fastcall GhostFlash(IChar IPlayer, int pPacket, int pPos)
{
    int pSkill = IPlayer.GetSkillPointer(14);

    if (IPlayer.IsValid() && pSkill)
    {
        ISkill xSkill((void*)pSkill);
        int nSkillGrade = xSkill.GetGrade();
        int nTargetID = 0;
        char bType = 0;
        void *pTarget = 0;
        CPacket::Read((char*)pPacket, (char*)pPos, "bd", &bType, &nTargetID);
        int nMana = (13 + (nSkillGrade * 2));

        if (bType == 0 && nTargetID)
            pTarget = CPlayer::FindPlayer(nTargetID);

        if (bType == 1 && nTargetID)
            pTarget = CMonster::FindMonster(nTargetID);

        if (bType >= 2)
            return;

        if (pTarget && nSkillGrade && IPlayer.IsValid())
        {
            IChar Target(pTarget);

            if (IPlayer.GetCurMp() < nMana)
                return;

            if (pTarget == IPlayer.GetOffset())
                return;

            if (IPlayer.IsValid() && Target.IsValid())
            {
                if (!IPlayer.IsInRange(Target,300))
                    return;

                int nDmg = (IPlayer.GetMaxMagAttack() +
                        ((IPlayer.GetMaxMagAttack() +
                          CChar::GetInt((int)IPlayer.GetOffset())) +
                         (CChar::GetWis((int)IPlayer.GetOffset()) +
                          (nSkillGrade * 60)))) + (IPlayer.GetLevel() * 7);

                if (Target.GetType() == 0)
                    nDmg = (nDmg * SHMGF) / 100;

                IPlayer.OktayDamageSingle(Target,nDmg,14);
                IPlayer.SetDirection(Target);
                IPlayer.DecreaseMana(nMana);
            }
        } else {
            IPlayer.CouldntExecuteSkill();
        }
    }
}
