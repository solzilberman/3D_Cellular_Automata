for a in {1..14}
do
    sed -i "3s/.*/RULENUM:$a/" ./config
done

