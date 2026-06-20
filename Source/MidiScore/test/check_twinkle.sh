# Check twinkle output is unchanged
../midiscore ../files/twinkle.txt --timesig 4/4 --bpm 60 --quant c > twinkle_out.txt
diff twinkle_out.txt twinkle_good.txt

