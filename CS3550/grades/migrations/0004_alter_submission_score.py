# Generated by Django 4.2.4 on 2023-10-21 07:07

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('grades', '0003_alter_submission_unique_together'),
    ]

    operations = [
        migrations.AlterField(
            model_name='submission',
            name='score',
            field=models.FloatField(blank=True, default=0.0, null=True),
        ),
    ]
